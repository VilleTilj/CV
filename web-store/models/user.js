const bcrypt = require('bcryptjs');
const mongoose = require('mongoose');
const Schema = mongoose.Schema;


const userSchema = new Schema({
  // TODO: 9.4 Implement this
	name: {type: String, required: true, trim: true, maxlength: 50},	
	email: {type: String, required: true, unique: true, match: /[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?/},
	password: {type: String, required: true, minlength: 10, set: function(v){ if(v.length >9){return bcrypt.hashSync(v)}}},
	role: {type: String, required: true, enum: ["customer", "admin"], lowercase: true, trim: true, default: "customer"}
});

/**
 * Compare supplied password with user's own (hashed) password
 *
 * @param {string} password
 * @returns {Promise<boolean>} promise that resolves to the comparison result
 */
userSchema.methods.checkPassword = async function (password) {
  // TODO: 9.4 Implement this
	const comparison = bcrypt.compareSync(password, this.password);
	return comparison;
};

// Omit the version key when serialized to JSON
userSchema.set('toJSON', { virtuals: false, versionKey: false });

const User = new mongoose.model('User', userSchema);
module.exports = User;
